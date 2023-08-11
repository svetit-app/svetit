import { ComponentFixture, TestBed } from '@angular/core/testing';

import { EditSchemeGroupComponent } from './edit-scheme-group.component';

describe('EditSchemeGroupComponent', () => {
  let component: EditSchemeGroupComponent;
  let fixture: ComponentFixture<EditSchemeGroupComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ EditSchemeGroupComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(EditSchemeGroupComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
