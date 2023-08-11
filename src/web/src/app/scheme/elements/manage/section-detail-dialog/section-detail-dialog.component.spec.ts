import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SectionDetailDialogComponent } from './section-detail-dialog.component';

describe('SectionDetailModalComponent', () => {
  let component: SectionDetailDialogComponent;
  let fixture: ComponentFixture<SectionDetailDialogComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SectionDetailDialogComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SectionDetailDialogComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
