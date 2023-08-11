import { ComponentFixture, TestBed } from '@angular/core/testing';

import { MnemoschemeComponent } from './mnemoscheme.component';

describe('MnemoschemeComponent', () => {
  let component: MnemoschemeComponent;
  let fixture: ComponentFixture<MnemoschemeComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ MnemoschemeComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(MnemoschemeComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
